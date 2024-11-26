package cz.cvut.fit.tjv.realestates.controller;

import cz.cvut.fit.tjv.realestates.application.OwnerService;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.autoconfigure.web.servlet.AutoConfigureMockMvc;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.boot.test.mock.mockito.MockBean;
import org.springframework.test.web.servlet.MockMvc;

@SpringBootTest
@AutoConfigureMockMvc
public class OwnerControllerIntegrationTest {
    @Autowired
    private MockMvc mvc;
    @MockBean
    private OwnerService ownerService;

    @Test
    public void testOwnerGetAll() {

    }

    @Test
    public void testOwnerGet() {

    }

    @Test
    public void testOwnerCreate() {

    }

    @Test
    public void testOwnerUpdate() {

    }

    @Test
    public void testOwnerDelete() {

    }


}
