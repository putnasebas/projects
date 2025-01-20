package cz.cvut.fit.tjv.realestates.client.ui;

import cz.cvut.fit.tjv.realestates.client.data.PropertyClient;
import cz.cvut.fit.tjv.realestates.client.model.CustomerDTO;
import cz.cvut.fit.tjv.realestates.client.model.OwnerDTO;
import cz.cvut.fit.tjv.realestates.client.model.PropertyDTO;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.*;


@Controller
@RequestMapping("/rest/api/property")
public class PropertyWebController {
    private final PropertyClient propertyClient;

    public PropertyWebController(PropertyClient propertyClient) {
        this.propertyClient = propertyClient;
    }

    @GetMapping("/{id}")
    public String getProperty(@PathVariable Long id, Model model) {
        model.addAttribute("newOwner", new OwnerDTO());
        model.addAttribute("newCustomer", new CustomerDTO());
        model.addAttribute("property", propertyClient.getPropertyById(id));
        return "Property/PropertyById";
    }

    @GetMapping
    public String getAllProperties(Model model) {
        model.addAttribute("properties", propertyClient.getAllProperties());
        model.addAttribute("firstEmail", "");
        model.addAttribute("secondEmail", "");
        model.addAttribute("newProperty", new PropertyDTO());
        return "Property/Properties";
    }

    @PostMapping
    public String addProperty(@ModelAttribute PropertyDTO propertyDTO, Model model) {
        model.addAttribute(propertyClient.createProperty(propertyDTO));
        return "redirect:/rest/api/property";
    }

    @PostMapping("/{id}/update")
    public String updateProperty(@PathVariable("id") Long id, @ModelAttribute PropertyDTO propertyDTO, Model model) {
        propertyDTO.setId(id);
        model.addAttribute("updateProperty", propertyClient.updatePropertyById(propertyDTO, id));
        return "redirect:/rest/api/property";
    }

    @GetMapping("/owners/intersection")
    public String getOwnersIntersection(@RequestParam("o1") String firstEmail, @RequestParam("o2") String secondEmail, Model model) {
        if(firstEmail == null || secondEmail == null) {
            throw new IllegalArgumentException("firstEmail and secondEmail are required");
        }
        model.addAttribute("firstOwner", firstEmail);
        model.addAttribute("secondOwner", secondEmail);
        model.addAttribute("intersection", propertyClient.getPropertiesOwnedByBothOwners(firstEmail, secondEmail));
        return "Property/OwnersIntersection";
    }

    @PostMapping("/{id}")
    public String deleteProperty(@PathVariable("id") Long id, Model model) {
        model.addAttribute(propertyClient.deleteProperty(id));
        return "redirect:/rest/api/property";
    }

    @PostMapping("/{id}/customer/add")
    public String addCustomer(@PathVariable("id") Long id, @ModelAttribute CustomerDTO customerDTO, Model model) {
        model.addAttribute("addCustomerToProperty", propertyClient.addCustomerToProperty(id, customerDTO));
        return "redirect:/rest/api/property/" + id;
    }

    @PostMapping("/{id}/customer")
    public String deleteCustomer(@PathVariable("id") Long id, Model model) {
        model.addAttribute("removeCustomerFromProperty", propertyClient.removeCustomerFromProperty(id));
        return "redirect:/rest/api/property/" + id;
    }

    @PostMapping("/{id}/owners")
    public String addOwner(@PathVariable("id") Long id, @ModelAttribute OwnerDTO ownerDTO, Model model) {
        model.addAttribute("addOwnerToProperty", propertyClient.addOwnerToProperty(id, ownerDTO));
        return "redirect:/rest/api/property/" + id;
    }

    @PostMapping("/{id}/owners/{id_owner}")
    public String deleteOwner(@PathVariable("id") Long id, @PathVariable("id_owner") String ownerEmail, Model model) {
        model.addAttribute(propertyClient.removeOwnerFromProperty(id, ownerEmail));
        return "redirect:/rest/api/property";
    }

}
