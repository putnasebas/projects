package cz.cvut.fit.tjv.realestates.application;

import cz.cvut.fit.tjv.realestates.domain.Owner;
import cz.cvut.fit.tjv.realestates.domain.Property;
import cz.cvut.fit.tjv.realestates.repository.JPAOwnerRepository;
import cz.cvut.fit.tjv.realestates.repository.JPAPropertyRepository;
import jakarta.persistence.EntityExistsException;
import jakarta.persistence.EntityNotFoundException;
import jakarta.transaction.Transactional;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.List;

@Service
@Transactional
public class OwnerServiceImpl implements OwnerService {
    private final JPAOwnerRepository ownerRepository;

    public OwnerServiceImpl(JPAOwnerRepository ownerRepository) {
        this.ownerRepository = ownerRepository;
    }

    @Override
    public Owner getEntityById(String email) throws EntityNotFoundException {
        return ownerRepository.findById(email).orElseThrow(() -> new EntityNotFoundException("Owner " + email + " not found"));
    }

    @Override
    public List<Owner> getAllEntities() {
        return ownerRepository.findAll();
    }

    @Override
    public List<Owner> getEntitiesByIds(List<String> ids) {
        if(ids == null || ids.isEmpty()) return new ArrayList<>();
        List<Owner> owners = new ArrayList<>();
        for (String id : ids) {
            if(!ownerRepository.existsById(id)) {
                throw new EntityNotFoundException("Owner's email " + id + " not found");
            }
            owners.add(getEntityById(id));
        }
        return owners;
    }

    @Override
    public Owner createEntity(Owner owner) throws EntityExistsException, IllegalArgumentException {
        if(ownerRepository.existsById(owner.getEmail())) {
            throw new EntityExistsException("Owner " + owner.getEmail() + " already exists");
        }
        if(owner.getEmail().contains("@mail.com")) {
            return ownerRepository.save(owner);
        }
        throw new IllegalArgumentException(owner.getEmail() + " is an invalid email address");
    }

    @Override
    public Owner updateEntity(Owner owner) throws EntityNotFoundException, IllegalArgumentException {
        if(ownerRepository.existsById(owner.getEmail())) {
            // Check if phone number is valid
            if(owner.getPhoneNumber() != null) {
                for(char c : owner.getPhoneNumber().toCharArray()) {
                    if(!Character.isDigit(c)) {
                        throw new IllegalArgumentException(owner.getEmail() + " has an invalid phone number");
                    }
                }
            }
            Owner oldOwner = ownerRepository.getReferenceById(owner.getEmail());
            Owner newOwner = new Owner(oldOwner.getEmail(), owner.getPhoneNumber(), oldOwner.getOwnedProperties());
            return ownerRepository.save(newOwner);
        }
        throw new EntityNotFoundException("Owner " + owner.getEmail() + " not found");
    }

    @Override
    public void deleteEntity(String email) throws EntityNotFoundException {
        if (ownerRepository.existsById(email)) {
            ownerRepository.deleteById(email);
            return;
        }
        throw new EntityNotFoundException("Owner " + email + " not found");
    }
}
